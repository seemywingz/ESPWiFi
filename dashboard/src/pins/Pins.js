import React, { useState, useEffect } from "react";
import Pin from "./Pin";
import { Container, Fab, Dialog, DialogTitle, DialogContent, DialogActions, Button, MenuItem, Select, FormControl, InputLabel } from "@mui/material";
import AddIcon from '@mui/icons-material/Add';

export default function Pins({ config, saveConfig }) {
    const [pins, setPins] = useState({}); // Initialize with an empty object
    const [isModalOpen, setIsModalOpen] = useState(false); // Modal visibility state
    const [selectedPinNum, setSelectedPinNum] = useState(""); // Selected pin number in modal
    const [selectedPinLabel, setSelectedPinLabel] = useState(""); // Selected pin label in modal

    // Define available ESP8266 and ESP32-S3-Zero pins

    const allPins = [
        { label: "GPIO 0", value: 0 },
        { label: "GPIO 1", value: 1 },
        { label: "GPIO 2", value: 2 },
        { label: "GPIO 3", value: 3 },
        { label: "GPIO 4", value: 4 },
        { label: "GPIO 5", value: 5 },
        { label: "GPIO 6", value: 6 },
        { label: "GPIO 7", value: 7 },
        { label: "GPIO 8", value: 8 },
        { label: "GPIO 9", value: 9 },
        { label: "GPIO 10", value: 10 },
        { label: "GPIO 11", value: 11 },
        { label: "GPIO 12", value: 12 },
        { label: "GPIO 13", value: 13 },
        { label: "GPIO 14", value: 14 },
        { label: "GPIO 15", value: 15 },
        { label: "GPIO 16", value: 16 },
        { label: "GPIO 17", value: 17 },
        { label: "GPIO 18", value: 18 },
        { label: "GPIO 19", value: 19 },
        { label: "GPIO 20", value: 20 },
        { label: "GPIO 21", value: 21 },
        { label: "GPIO 26", value: 26 },
        { label: "GPIO 27", value: 27 },
        { label: "GPIO 28", value: 28 },
        { label: "GPIO 29", value: 29 },
        { label: "GPIO 30", value: 30 },
        { label: "GPIO 31", value: 31 },
        { label: "GPIO 32", value: 32 },
        { label: "GPIO 33", value: 33 },
        { label: "GPIO 34", value: 34 },
        { label: "GPIO 35", value: 35 },
        { label: "GPIO 36", value: 36 },
        { label: "GPIO 37", value: 37 },
        { label: "GPIO 38", value: 38 },
        { label: "GPIO 39", value: 39 },
        { label: "GPIO 40", value: 40 },
        { label: "GPIO 41", value: 41 },
        { label: "GPIO 42", value: 42 },
        { label: "GPIO 43", value: 43 },
        { label: "GPIO 44", value: 44 },
        { label: "GPIO 45", value: 45 },
        { label: "GPIO 46", value: 46 },
    ];

    // Filter available pins
    const pinOptions = allPins.filter(pin => !Object.keys(pins).includes(pin.value.toString()));

    useEffect(() => {
        if (config && config.pins) {
            setPins(config.pins);
        }
    }, [config]);

    useEffect(() => {
        if (pinOptions.length > 0 && !pinOptions.some(pin => pin.value === selectedPinNum)) {
            setSelectedPinNum(pinOptions[0].value); // Default to the first available pin
        }
    }, [pinOptions, selectedPinNum]);

    const updatePins = (pinState, deletePin) => {
        if (deletePin) {
            const updatedPins = { ...pins };
            delete updatedPins[pinState.num];
            setPins(updatedPins);
            saveConfig({ ...config, pins: updatedPins });
            return;
        }
        const updatedPins = { ...pins };
        updatedPins[pinState.num] = pinState;
        setPins(updatedPins);
        saveConfig({ ...config, pins: updatedPins });
    };

    const handleAddBtnClick = () => {
        setSelectedPinNum(""); // Reset the selected pin number
        setIsModalOpen(true); // Open the modal when the Fab button is pressed
    };

    const handleCloseModal = () => {
        setIsModalOpen(false); // Close the modal
    };

    const handleAddPin = () => {
        const newPinState = {
            state: "low",
            name: selectedPinLabel,
            num: parseInt(selectedPinNum, 10),
            mode: "out", // Default mode
            duty: 1860,
            cycle: 20000,
            hz: 50,
        };
        updatePins(newPinState);
        handleCloseModal(); // Close the modal after submission
    };

    const handlePinNumberChange = (event) => {
        const selectedValue = event.target.value;
        const selectedLabel = allPins.find(pin => pin.value === selectedValue)?.label || "";
        setSelectedPinNum(selectedValue); // Update selected pin number
        setSelectedPinLabel(selectedLabel); // Update selected pin label
    };

    // Map over the keys of the pins object
    const pinElements = Object.keys(pins).map((key) => {
        const pin = pins[key];
        return <Pin key={key} config={config} pinNum={key} props={pin} updatePins={updatePins} />;
    });

    return (
        <Container sx={{ display: 'flex', flexWrap: 'wrap', justifyContent: 'center' }}>
            <Fab
                size="small"
                color="primary"
                aria-label="add"
                onClick={handleAddBtnClick}
                sx={{ position: 'fixed', top: '20px', right: '20px' }}
            >
                <AddIcon />
            </Fab>

            {pinElements}

            <Dialog open={isModalOpen} onClose={handleCloseModal}>
                <DialogTitle>Add Pin</DialogTitle>
                <DialogContent>
                    <FormControl fullWidth variant="outlined" sx={{ marginTop: 1 }}>
                        <InputLabel id="pin-select-label">Pin</InputLabel>
                        <Select
                            labelId="pin-select-label"
                            value={pinOptions.some(pin => pin.value === selectedPinNum) ? selectedPinNum : ""}
                            label="Pin Number"
                            onChange={handlePinNumberChange}
                        >
                            {pinOptions.map(pin => (
                                <MenuItem key={pin.value} value={pin.value}>{pin.label}</MenuItem>
                            ))}
                        </Select>
                    </FormControl>
                </DialogContent>
                <DialogActions>
                    <Button onClick={handleCloseModal} color="error">Cancel</Button>
                    <Button onClick={handleAddPin} color="primary" disabled={!selectedPinNum}>Add Pin</Button>
                </DialogActions>
            </Dialog>
        </Container>
    );
}

