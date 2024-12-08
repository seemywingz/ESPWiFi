import React, { useState } from 'react';

function GPIOSettings({ config, updateConfig: saveConfig }) {


    const toggleLight = () => {
        console.log('Toggling light');
        fetch('/gpio/on', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ light: 'toggle' })
        })
            .then(response => {
                if (response.ok) {
                    alert('Light toggled');
                } else {
                    alert('Failed to toggle light');
                }
            })
            .catch(error => {
                console.error('Error toggling light:', error);
                alert('Error toggling light');
            });
    }


    return (
        <div class="setting">
            <button onClick={toggleLight} >Light</button>
        </div >
    );
}

export default GPIOSettings;