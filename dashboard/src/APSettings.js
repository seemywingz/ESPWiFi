import React, { useState } from 'react';

function APSettings({ config, updateConfig }) {
    const [apSSID, setApSSID] = useState(config.ap.ssid);
    const [apPassword, setApPassword] = useState(config.ap.password);

    const handleSave = () => {
        updateConfig({ ...config, client: { apSSID, apPassword } });
    };

    return (
        <div class="setting">
            <label>SSID</label>
            <input value={apSSID} onChange={e => setApSSID(e.target.value)} />
            <label>Password</label>
            <input value={apPassword} onChange={e => setApPassword(e.target.value)} />
            <button onClick={handleSave} style={{ backgroundColor: 'brown' }}>Save</button>
        </div>
    );
}

export default APSettings;
