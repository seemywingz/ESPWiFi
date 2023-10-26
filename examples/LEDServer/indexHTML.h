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
            /* Medium Dark Gray */
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
            /* Darker Gray on Hover */
            box-shadow: 0px 5px 15px 0px rgba(0, 0, 0, 0.3);
        }

        .info-button {
            background-color: #444;
            /* Medium Dark Gray - Same as Toggle Button */
        }

        .info-container {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background-color: rgba(0, 0, 0, 0.5);
            display: none;
            align-items: center;
            justify-content: center;
            z-index: 1;
        }

        .info-box {
            background-color: #fff;
            padding: 20px;
            border-radius: 15px;
            box-shadow: 0px 5px 15px 0px rgba(0, 0, 0, 0.3);
            max-width: 80%;
            max-height: 80%;
            overflow: auto;
            font-size: 18px;
            line-height: 1.5;
            color: #333;
            /* Dark Grey Text Color */
        }

        .close-button,
        .info-button {
            position: absolute;
            top: 10px;
            right: 10px;
            background-color: #444;
            /* Medium Dark Gray - Same as Toggle Button */
            border: none;
            color: #fff;
            padding: 10px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 18px;
            margin: 10px;
            cursor: pointer;
            border-radius: 10px;
            transition: background-color 0.3s ease, box-shadow 0.3s ease;
        }

        .close-button:hover,
        .info-button:hover {
            background-color: #555;
            /* Darker Gray on Hover - Same as Toggle Button */
            box-shadow: 0px 5px 15px 0px rgba(0, 0, 0, 0.3);
        }
    </style>
</head>

<body>
    <div class="container">
        <div class="header">LED Web Controls</div>
        <div class="button-container">
            <button id="toggleButton" class='button'>Loading...</button>
            <button id="infoButton" class='button info-button'>Info</button>
        </div>
    </div>

    <div class="info-container" id="infoContainer">
        <div class="info-box">
            <button class="close-button" id="closeButton">Close</button>
            <p id="boardInfo"></p>
        </div>
    </div>

    <script>
        var button = document.getElementById('toggleButton');
        var infoButton = document.getElementById('infoButton');
        var infoContainer = document.getElementById('infoContainer');
        var closeButton = document.getElementById('closeButton');
        var boardInfo = document.getElementById('boardInfo');

        function updateButton(isLedOn) {
            button.textContent = 'Turn ' + (isLedOn ? 'Off' : 'On');
        }

        function toggleLED() {
            fetch('/led/toggle', { method: 'GET' })
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                    var isLedOn = data.includes('on'); // Check if the response contains 'on'
                    updateButton(isLedOn);
                });
        }

        // Fetch the initial state of the LED when the page is loaded
        fetch('/led/state', { method: 'GET' })
            .then(response => response.text())
            .then(data => {
                var isLedOn = data.includes('on'); // Check if the response contains 'on'
                updateButton(isLedOn);
                // Attach click event listener after setting the initial button state
                button.addEventListener('click', toggleLED);
            })
            .catch(error => {
                console.error('Error fetching LED state:', error);
            });

        // Show the info container when the info button is clicked
        infoButton.addEventListener('click', function () {
            fetch('/info', { method: 'GET' })
                .then(response => response.text())
                .then(data => {
                    // Replace newline characters with <br> elements for proper line breaks
                    boardInfo.innerHTML = data.replace(/\n/g, '<br>');
                    infoContainer.style.display = 'flex';
                })
                .catch(error => {
                    console.error('Error fetching board info:', error);
                });
        });

        // Hide the info container when the close button is clicked
        closeButton.addEventListener('click', function () {
            infoContainer.style.display = 'none';
        });
    </script>
</body>

</html>
)rawliteral";