import React, { useState, useEffect } from "react";
import Pin from "./Pin";
import { Container, Fab, Dialog, DialogTitle, DialogContent, DialogActions, Button, MenuItem, Select, FormControl, InputLabel } from "@mui/material";
import AddIcon from '@mui/icons-material/Add';

export default function Pins({ config }) {
    const [pins, setPins] = useState({}); // Initialize with an empty object
    const [isModalOpen, setIsModalOpen] = useState(false); // Modal visibility state
    const [selectedPinNum, setSelectedPinNum] = useState(""); // Selected pin number in modal

    useEffect(() => {
        if (config && config.pins) {
            setPins(config.pins);
        }
    }, [config]);

    const handleDeletePin = (pinNum) => {
        const updatedPins = { ...pins };
        delete updatedPins[pinNum];
        setPins(updatedPins);
        setSelectedPinNum(""); // Clear the selection when a pin is deleted
    };

    const handleUpdatePin = (pinState, method) => {
        fetch(`http://${config.hostname}:${config.port}/api/pin`, {
            method: method,
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(pinState),
        }).then(response => {
            if (response.ok) {
                if (method === "DELETE") {
                    handleDeletePin(pinState.num);
                } else if (method === "POST" || method === "PUT") {
                    setPins(prevPins => ({
                        ...prevPins,
                        [pinState.num]: pinState,
                    }));
                }
                setSelectedPinNum(""); // Clear the selection after a successful update
            }
        }).catch(error => {
            console.error('Error updating pin:', error);
        });
    };

    const handleFabClick = () => {
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
        handleUpdatePin(newPinState, "PUT");
        handleCloseModal(); // Close the modal after submission
    };

    const handlePinNumberChange = (event) => {
        setSelectedPinNum(event.target.value); // Update selected pin number
    };

    // Generate an array of pin numbers from 0 to 25, excluding the ones already in use
    const pinOptions = Array.from({ length: 26 }, (_, index) => index)
        .filter(pinNum => !pins.hasOwnProperty(pinNum.toString())); // Exclude pins already implemented

    // Map over the keys of the pins object
    const pinElements = Object.keys(pins).map((key) => {
        const pin = pins[key];
        return <Pin key={key} config={config} pinNum={key} props={pin} onUpdate={handleUpdatePin} />;
    });

    return (
        <Container sx={{
            display: 'flex',
            flexWrap: 'wrap',
            justifyContent: 'center',
        }}>
            <Fab size="small" color="primary" aria-label="add"
                onClick={handleFabClick}
                sx={{
                    position: 'fixed',
                    top: '20px',
                    left: '20px',
                }}>
                <AddIcon />
            </Fab>

            {pinElements}

            <Dialog open={isModalOpen} onClose={handleCloseModal}>
                <DialogTitle>Add Pin</DialogTitle>
                <DialogContent>
                    <FormControl fullWidth variant="outlined" sx={{ marginTop: 1 }}> {/* Add margin to move the Select down */}
                        <InputLabel id="pin-select-label">Pin Number</InputLabel>
                        <Select
                            labelId="pin-select-label"
                            value={selectedPinNum || ""}
                            label="Pin Number"
                            onChange={handlePinNumberChange}
                        >
                            {pinOptions.map(pinNum => (
                                <MenuItem key={pinNum} value={pinNum}>{pinNum}</MenuItem>
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
