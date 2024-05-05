import React, { useState } from 'react';

function APSettings({ config, updateConfig }) {
    const [apSSID, setApSSID] = useState(config.ap.ssid);
    const [apPassword, setApPassword] = useState(config.ap.password);

    const handleSave = () => {
        updateConfig({ ...config, client: { apSSID, apPassword } });
    };

    const restartAsAP = () => {
        const newConfig = { ...config, mode: 'ap' };
        updateConfig(newConfig);
        fetch('/restart', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(newConfig)
        })
            .then(response => {
                if (response.ok) {
                    alert('System is restarting as Access Point...');
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
            <input value={apSSID} onChange={e => setApSSID(e.target.value)} />
            <label>Password</label>
            <input value={apPassword} onChange={e => setApPassword(e.target.value)} />
            <button onClick={handleSave} style={{ backgroundColor: 'brown' }}>Save</button>
            <button onClick={restartAsAP} style={{ backgroundColor: 'brown', marginTop: '10px' }}>Restart as Access Point</button>
        </div>
    );
}

export default APSettings;
