import React, { useState } from 'react';

function WiFiSettings({ config, updateConfig }) {
    const [ssid, setSSID] = useState(config.client.ssid);
    const [password, setPassword] = useState(config.client.password);

    const handleSave = () => {
        updateConfig({ ...config, client: { ssid, password } });
    };

    const restartAsClient = () => {
        const newConfig = { ...config, mode: 'client' };
        updateConfig(newConfig);
        fetch('/restart', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(newConfig)
        })
            .then(response => {
                if (response.ok) {
                    alert('System is restarting as client...');
                } else {
                    alert('Failed to restart system.');
                }
            })
            .catch(error => {
                console.error('Error restarting system:', error);
                alert('Error restarting system');
            });
    };

    return (
        <div class="setting">
            <label>SSID</label>
            <input value={ssid} onChange={e => setSSID(e.target.value)} />
            <label>Password</label>
            <input value={password} onChange={e => setPassword(e.target.value)} />
            <button onClick={handleSave} style={{ backgroundColor: 'brown' }}>Save</button>
            <button onClick={restartAsClient} style={{ backgroundColor: 'brown', marginTop: '10px' }}>Restart as Client</button>
        </div >
    );
}

export default WiFiSettings;