#NOTE: this version works only on linux with dbus and bluez!

import sys
import time
import dbus
import dbus.mainloop.glib
from gi.repository import GLib

# Constants
ADAPTER_PATH = '/org/bluez/hci0'
BLUEZ_SERVICE = 'org.bluez'
DEVICE_INTERFACE = 'org.bluez.Device1'
ADAPTER_INTERFACE = 'org.bluez.Adapter1'
PROPS_INTERFACE = 'org.freedesktop.DBus.Properties'
TARGET_NAME = "Wireless Controller"

class PS5Connector:
    def __init__(self):
        # 1. Setup DBus and MainLoop
        dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
        self.bus = dbus.SystemBus()
        self.mainloop = GLib.MainLoop()
        
        # 2. Get Adapter Object
        try:
            self.adapter = self.bus.get_object(BLUEZ_SERVICE, ADAPTER_PATH)
            self.adapter_props = dbus.Interface(self.adapter, PROPS_INTERFACE)
            self.adapter_methods = dbus.Interface(self.adapter, ADAPTER_INTERFACE)
        except dbus.exceptions.DBusException:
            print("[-] Error: Could not find Bluetooth Adapter. Is it plugged in?")
            sys.exit(1)

    def power_on_adapter(self):
        """Forces the Bluetooth radio ON."""
        print("[*] Powering on Adapter...")
        self.adapter_props.Set(ADAPTER_INTERFACE, "Powered", True)
        
        # Verify it worked
        powered = self.adapter_props.Get(ADAPTER_INTERFACE, "Powered")
        if not powered:
            print("[-] Failed to power on adapter. Check 'rfkill list'.")
            sys.exit(1)

    def clean_old_devices(self):
        """Removes the PS5 controller if it's already cached to ensure a fresh pair."""
        managed_objects = self.bus.get_object(BLUEZ_SERVICE, "/").GetManagedObjects(dbus_interface="org.freedesktop.DBus.ObjectManager")
        
        for path, interfaces in managed_objects.items():
            if DEVICE_INTERFACE in interfaces:
                props = interfaces[DEVICE_INTERFACE]
                name = props.get("Name", "Unknown")
                if TARGET_NAME in name:
                    print(f"[*] Found old cached device {name} at {path}. Removing...")
                    try:
                        self.adapter_methods.RemoveDevice(path)
                    except dbus.exceptions.DBusException as e:
                        print(f"[-] Error removing device: {e}")

    def start_scan(self):
        """Starts the discovery process and sets up the signal listener."""
        # Set filter to Classic Bluetooth (BREDR) only - Critical for PS5
        filter_dict = dbus.Dictionary({'Transport': 'bredr'}, signature='sv')
        self.adapter_methods.SetDiscoveryFilter(filter_dict)

        print("[*] Starting Discovery (Waiting for Controller)...")
        self.adapter_methods.StartDiscovery()

        # Add a listener: When a NEW interface (device) is added, call device_found()
        self.bus.add_signal_receiver(
            self.device_found,
            dbus_interface="org.freedesktop.DBus.ObjectManager",
            signal_name="InterfacesAdded"
        )
        
        print("[*] Script is listening. Put your PS5 Controller in pairing mode NOW (Create + PS).")

    def device_found(self, path, interfaces):
        """Callback triggered when a new device is discovered."""
        if DEVICE_INTERFACE in interfaces:
            props = interfaces[DEVICE_INTERFACE]
            name = props.get("Name", "Unknown")
            address = props.get("Address", "Unknown")
            
            if TARGET_NAME in name:
                print(f"\n[+] FOUND PS5 Controller: {name} [{address}]")
                print(f"[*] Stopping scan to stabilize connection...")
                self.adapter_methods.StopDiscovery()
                
                # Launch the connection sequence
                self.connect_sequence(path)

    def connect_sequence(self, path):
        """Handles Trust -> Pair -> Connect."""
        device = self.bus.get_object(BLUEZ_SERVICE, path)
        device_props = dbus.Interface(device, PROPS_INTERFACE)
        device_methods = dbus.Interface(device, DEVICE_INTERFACE)

        # 1. TRUST
        print("[*] Trusting device...")
        device_props.Set(DEVICE_INTERFACE, "Trusted", True)

        # 2. PAIR
        print("[*] Attempting to Pair...")
        try:
            device_methods.Pair()
            print("[+] Pairing Successful!")
        except dbus.exceptions.DBusException as e:
            if "AlreadyExists" in str(e):
                print("[!] Device already paired. Moving to connect.")
            elif "AuthenticationFailed" in str(e):
                print("[-] Authentication Failed! BlueZ needs an Agent.")
                print("    > FIX: Run 'bluetoothctl' in a separate terminal window to handle the PIN.")
                return
            else:
                print(f"[-] Pairing Error: {e}")
                return

        # 3. CONNECT
        print("[*] Connecting (HID)...")
        # Give BlueZ a moment to finalize the pair before connecting
        time.sleep(2)
        try:
            device_methods.Connect()
            print("\n[SUCCESS] Controller is Connected! LED should be Blue.")
            self.mainloop.quit() # Exit the script
        except dbus.exceptions.DBusException as e:
            print(f"[-] Connection Failed: {e}")
            self.mainloop.quit()

    def run(self):
        self.power_on_adapter()
        self.clean_old_devices()
        self.start_scan()
        try:
            self.mainloop.run()
        except KeyboardInterrupt:
            print("\n[*] Stopping...")
            self.adapter_methods.StopDiscovery()

if __name__ == "__main__":
    app = PS5Connector()
    app.run()
