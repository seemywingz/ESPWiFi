import React, { useState, useEffect } from "react";
import { Container, Fab, Dialog, DialogTitle, DialogContent, DialogActions, Button, MenuItem, Select, FormControl, InputLabel } from "@mui/material";
import SettingsIcon from '@mui/icons-material/Settings'; // Import gear icon

export default function Pins({ config }) {
    const [isModalOpen, setIsModalOpen] = useState(false); // Modal visibility state
    const [selectedPinNum, setSelectedPinNum] = useState(""); // State for selected pin number
    const pinOptions = [1, 2, 3, 4, 5]; // Example pin options

    useEffect(() => {
        if (config) {
            // ...existing code...
        }
    }, [config]);

    const handleOpenModal = () => {
        setIsModalOpen(true);
    };

    const handleCloseModal = () => {
        setIsModalOpen(false);
    };

    const handleModalSubmit = () => {
        // Handle the submit action
        handleCloseModal();
    };

    return (
        <Container sx={{
            display: 'flex',
            flexWrap: 'wrap',
            justifyContent: 'center',
        }}>
            <Fab size="small" color="primary" aria-label="add"
                onClick={handleOpenModal}
                sx={{
                    position: 'fixed',
                    top: '20px',
                    left: '20px',
                }}>
                <SettingsIcon />
            </Fab>

            <Dialog open={isModalOpen} onClose={handleCloseModal}>
                <DialogTitle>Settings</DialogTitle>
                <DialogContent>

                </DialogContent>
                <DialogActions>
                    <Button onClick={handleCloseModal} color="error">Cancel</Button>
                </DialogActions>
            </Dialog>
        </Container>
    );
}
