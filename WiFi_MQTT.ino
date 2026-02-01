void mqttPublishAll() {
  char payload[16];

  if (!client.connected()) connectMQTT();
  if (!client.connected()) return;

  // Batt1
  snprintf(payload, sizeof(payload), "%.2f", battU[0]);
  client.publish(mqtt_topic_batt1, payload, true);
  client.publish(mqtt_topic_batt1stat, String(battstat[0]).c_str(), true);
  
  // Batt2
  snprintf(payload, sizeof(payload), "%.2f", battU[1]);
  client.publish(mqtt_topic_batt2, payload, true);
  client.publish(mqtt_topic_batt2stat, String(battstat[1]).c_str(), true);

  // Batt3
  snprintf(payload, sizeof(payload), "%.2f", battU[2]);
  client.publish(mqtt_topic_batt3, payload, true);
  client.publish(mqtt_topic_batt3stat, String(battstat[2]).c_str(), true);

  // Batt4
  snprintf(payload, sizeof(payload), "%.2f", battU[3]);
  client.publish(mqtt_topic_batt4, payload, true);
  client.publish(mqtt_topic_batt4stat, String(battstat[3]).c_str(), true);
}

void connectMQTT() {
  unsigned long lastAttempt = millis();
  while (!client.connected()) {
    // Build clientId from MAC (short, hex)
    uint64_t mac = ESP.getEfuseMac();
    char clientId[32];
    snprintf(clientId, sizeof(clientId), "esp32_%04X%04X", (uint16_t)(mac >> 32), (uint32_t)(mac & 0xFFFF));

    Serial.print("Versuche MQTT-Verbindung mit ID ");
    Serial.println(clientId);

    if (client.connect(clientId, mqtt_user, mqtt_pass)) {
      Serial.println("MQTT verbunden");
      // keine Subscribes erforderlich; falls gewünscht, setze Callback und subscribe hier.
    } else {
      Serial.print("MQTT Verbindung fehlgeschlagen, rc=");
      Serial.print(client.state());
      Serial.println(" - neuer Versuch in 2s");
      delay(2000);
    }

    // Safety: Falls zu lange kein Connect, breche und starte WiFi neu
    if (millis() - lastAttempt > 120000) {
      Serial.println("MQTT Connect Timeout, WiFi neu verbinden...");
      WiFi.disconnect();
      setupWiFiStatic();
      lastAttempt = millis();
    }
  }
}

void setupWiFiStatic() {
  Serial.print("Setze statische IP ... ");
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("Fehler bei IP-Definition");
  } else {
    Serial.println("ok");
  }
  WiFi.begin(ssid, wifi_pass);
  Serial.print("Verbinde zum WiFi");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 60) {
      Serial.println("\nWiFi Verbindung timeout, starte neu ...");
      ESP.restart();
    }
  }
  Serial.println();
  Serial.print("WiFi verbunden, IP: ");
  Serial.println(WiFi.localIP());
}