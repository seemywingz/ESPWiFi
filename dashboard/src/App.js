import React, { useState, useEffect } from 'react';
import WiFiSettings from './WiFiSettings';
import APSettings from './APSettings';

var defaultConfig = {
  "version": "0.0.1",
  "mode": "client",
  "mdns": "ESPWiFi",
  "client": {
    "ssid": "default-ssid",
    "password": "default-password"
  },
  "ap": {
    "ssid": "ESPWiFi-AP",
    "password": "default-abc123"
  },
  "openAI": {
    "apiKey": "",
    "voice": "onyx",
    "system_message": "You are a helpful assistant."
  }
};

function App() {
  var [config, setConfig] = useState(null);
  const [selectedSetting, setSelectedSetting] = useState('about');  // Default to 'about'

  useEffect(() => {
    fetch('/config')
      .then(response => response.json())
      .then(data => setConfig(data))
      .catch(error => console.error('Error loading configuration:', error));
  }, []);

  const updateConfig = (newConfig) => {
    fetch('/config', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(newConfig),
    }).then(response => {
      if (!response.ok) throw new Error('Failed to save configuration');
      return response.json();
    }).then(data => {
      setConfig(newConfig);
      console.log('Configuration updated:', data);
    }).catch(error => console.error('Error updating configuration:', error));
  };

  // if (!config) return <div>Loading...</div>;

  if (!config) {
    config = defaultConfig;
  }

  const renderSetting = () => {
    switch (selectedSetting) {
      case 'wifi':
        return <WiFiSettings config={config} updateConfig={updateConfig} />;
      case 'ap':
        return <APSettings config={config} updateConfig={updateConfig} />;
      // case 'openai':
      //   return <OpenAISettings config={config} updateConfig={updateConfig} />;
      // Add other cases as needed
      case 'about':
        return <div class="setting" id="about-settings">

          <div class="setting">
            <label>Version</label>
            <label id="version">{config.version}</label>
          </div>

          <div class="setting">
            <label>Designed and Developed by:</label>
            <a href="https://github.com/seemywingz/ESPWiFi-base"
              target="_blank" id="SeeMyWingZ">SeeMyWingZ</a>
          </div>

        </div>;
    }
  };

  return (
    <div className="container">
      <label className="header">ESPWiFi</label>
      <div className="setting">
        <select
          id="settingSelect"
          className="clickable"
          onChange={(e) => setSelectedSetting(e.target.value)}
        >
          <option value="about">About</option>
          <option value="wifi">WiFi</option>
          <option value="ap">AP</option>
          <option value="openai">OpenAI</option>
          <option value="files">Files</option>
        </select>
      </div>
      {renderSetting()}
    </div>
  );
}

export default App;