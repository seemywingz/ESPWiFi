import React, { useState, useEffect } from "react";
import Pin from "./Pin";
import { Container, Fab, Dialog, DialogTitle, DialogContent, DialogActions, Button, MenuItem, Select, FormControl, InputLabel } from "@mui/material";
import AddIcon from '@mui/icons-material/Add';

export default function Pins({ config, saveConfig }) {
    const [pins, setPins] = useState({}); // Initialize with an empty object
    const [isModalOpen, setIsModalOpen] = useState(false); // Modal visibility state
    const [selectedPinNum, setSelectedPinNum] = useState(""); // Selected pin number in modal

    useEffect(() => {
        if (config && config.pins) {
            setPins(config.pins);
        }
    }, [config]);

    const handleUpdatePin = (pinState) => {
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

    const handleModalSubmit = () => {
        // Create a new pin state and call handleUpdatePin with PUT method
        const newPinState = {
            on: false,
            name: `Pin ${selectedPinNum}`,
            num: parseInt(selectedPinNum, 10),
            mode: "out", // Default mode
        };
        handleUpdatePin(newPinState);
        handleCloseModal(); // Close the modal after submission
    };

    const handlePinNumberChange = (event) => {
        setSelectedPinNum(event.target.value); // Update selected pin number
    };

    const esp8266Pins = [
        { label: "D0 (GPIO16)", value: 16 },
        { label: "D1 (GPIO5)", value: 5 },
        { label: "D2 (GPIO4)", value: 4 },
        { label: "D3 (GPIO0)", value: "0" },
        { label: "D4 (GPIO2)", value: 2 },
        { label: "D5 (GPIO14)", value: 14 },
        { label: "D6 (GPIO12)", value: 12 },
        { label: "D7 (GPIO13)", value: 13 },
        { label: "D8 (GPIO15)", value: 15 },
        { label: "A0 (Analog)", value: "A0" } // Assuming 17 for analog pin
    ];

    const pinOptions = esp8266Pins.filter(pin => !pins.hasOwnProperty(pin.value.toString()));

    // Map over the keys of the pins object
    const pinElements = Object.keys(pins).map((key) => {
        const pin = pins[key];
        return <Pin key={key} config={config} pinNum={key} props={pin} updatePins={handleUpdatePin} />;
    });

    return (
        <Container sx={{
            display: 'flex',
            flexWrap: 'wrap',
            justifyContent: 'center',
        }}>
            <Fab size="small" color="primary" aria-label="add"
                onClick={handleAddBtnClick}
                sx={{
                    position: 'fixed',
                    top: '20px',
                    right: '20px',
                }}>
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
                            value={selectedPinNum || ""}
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
                    <Button onClick={handleModalSubmit} color="primary" disabled={!selectedPinNum}>Add Pin</Button>
                </DialogActions>
            </Dialog>
        </Container>
    );
}
