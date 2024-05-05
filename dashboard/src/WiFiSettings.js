import React, { useState } from 'react';

function WiFiSettings({ config, updateConfig }) {
    const [ssid, setSSID] = useState(config.client.ssid);
    const [password, setPassword] = useState(config.client.password);

    const handleSave = () => {
        updateConfig({ ...config, client: { ssid, password } });
    };

    return (
        <div class="setting">
            <label>SSID</label>
            <input value={ssid} onChange={e => setSSID(e.target.value)} />
            <label>Password</label>
            <input value={password} onChange={e => setPassword(e.target.value)} />
            <button onClick={handleSave} style={{ backgroundColor: 'brown' }}>Save</button>
        </div >
    );
}

export default WiFiSettings;