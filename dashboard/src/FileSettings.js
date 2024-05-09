import React, { useState, useEffect } from 'react';

function FileSettings() {
    const [files, setFiles] = useState([]);

    useEffect(() => {
        fetch('/list-files')
            .then(response => response.json())
            .then(data => {
                setFiles(data);
            })
            .catch(error => console.error('Failed to load files:', error));
    }, []);

    if (!files.length) {
        return (
            <div>Loading Files... ⚙️</div>
        );
    }

    return (
        <div className="setting">
            <ul id="fileList">
                {files.map((file, index) => (
                    <li key={index} >
                        {/* Link to download/view the file; adjust the href as needed */}
                        <a href={`/${file}`} target="_blank" rel="noopener noreferrer">{file}</a>
                    </li>
                ))}
            </ul>
        </div>
    );
}

export default FileSettings;
