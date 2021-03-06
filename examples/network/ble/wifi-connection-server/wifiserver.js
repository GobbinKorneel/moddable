/*
 * Copyright (c) 2016-2018  Moddable Tech, Inc.
 *
 *   This file is part of the Moddable SDK.
 * 
 *   This work is licensed under the
 *       Creative Commons Attribution 4.0 International License.
 *   To view a copy of this license, visit
 *       <https://creativecommons.org/licenses/by/4.0>.
 *   or send a letter to Creative Commons, PO Box 1866,
 *   Mountain View, CA 94042, USA.
 *
 */

import BLEServer from "bleserver";
import {uuid} from "btutils";
import WiFi from "wifi";
import Net from "net";

export default class WiFiServer extends BLEServer {
	onReady() {
		this.deviceName = "Moddable Device";
		this.onDisconnected();
	}
	onConnected() {
		this.stopAdvertising();
	}
	onDisconnected() {
		this.startAdvertising({
			advertisingData: {shortName: "Moddable"},
			scanResponseData: {flags: 6, completeName: this.deviceName, completeUUID16List: [uuid`FF00`]}
		});
	}
	onCharacteristicWritten(characteristic, value) {
		switch(characteristic.name) {
			case "SSID":
				this.ssid = value;
				break;
			case "password":
				this.password = value;
				break;
			case "control":
				if ((1 == value) && this.ssid) {
					this.close();
					this.connectToWiFiNetwork();
				}
				break;
		}
	}
	connectToWiFiNetwork() {
		trace(`Connecting to ${this.ssid}...\n`);
		let dictionary = { ssid:this.ssid };
		if (this.password)
			dictionary.password = this.password;
		let monitor = new WiFi(dictionary, msg => {
			switch (msg) {
				case "connect":
					break; // still waiting for IP address
				case "gotIP":
					trace(`IP address ${Net.get("IP")}\n`);
					break;
				case "disconnect":
					break;  // connection lost
			}
		})
	}
}
Object.freeze(WiFiServer.prototoype);

