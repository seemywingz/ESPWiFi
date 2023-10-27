const char indexHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>LED Web Controls</title>
    <style>
        body {
            background-color: #111;
            font-family: Arial, Helvetica, Sans-Serif;
            text-align: center;
            margin: 0;
            padding: 0;
            color: #fff;
        }
        .container {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
        }
        .header {
            font-size: 36px;
            margin-bottom: 20px;
        }
        .button-container {
            background-color: #111;
            padding: 20px;
            border-radius: 30px;
            margin: 20px;
            transition: box-shadow 0.3s ease;
        }
        .button {
            background-color: #444;
            border: none;
            color: #fff;
            padding: 20px 40px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 24px;
            margin: 10px;
            cursor: pointer;
            border-radius: 15px;
            transition: background-color 0.3s ease, box-shadow 0.3s ease;
        }
        .button:hover {
            background-color: #555;
            box-shadow: 0px 5px 15px 0px rgba(0, 0, 0, 0.3);
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">LED Web Controls</div>
        <div class="button-container">
            <button id="toggleButton" class='button'>Loading...</button>
        </div>
    </div>

    <script>
    var button = document.getElementById('toggleButton');

    function updateButton(isLedOn) {
        button.textContent = 'Turn ' + (isLedOn ? 'Off' : 'On');
    }

    function toggleLED() {
        console.log("Button clicked"); // Debugging line
        fetch('/led/toggle', { method: 'GET' })
            .then(response => response.text())
            .then(data => {
                console.log("Response:", data); // Debugging line
                var isLedOn = data.toLowerCase().includes('on'); // Case-insensitive check
                updateButton(isLedOn);
            })
            .catch(error => {
                console.error('Error toggling LED:', error);
                button.textContent = `Error: ${error.message}`;
            });
    }

    // Fetch the initial state of the LED when the page is loaded
    fetch('/led/state', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var isLedOn = data.toLowerCase().includes('on'); // Case-insensitive check
            updateButton(isLedOn);
            // Attach click event listener after setting the initial button state
            button.addEventListener('click', toggleLED);
        })
        .catch(error => {
            console.error('Error fetching LED state:', error);
            button.textContent = 'Error';
        });
    </script>
</body>
</html>
)rawliteral";
