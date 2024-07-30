const express = require('express');
const SensorController = require('../../controllers/Sensors');

const router = express.Router();

// Cria um novo sensor
router.post('/sensors', SensorController.createSensor);

// Obtém todos os sensores
router.get('/sensors', SensorController.getAllSensors);

// Obtém um sensor por ID
router.get('/sensors/:id', SensorController.getSensorById);

// Atualiza um sensor por ID
router.put('/sensors/:id', SensorController.updateSensor);

// Deleta um sensor por ID
router.delete('/sensors/:id', SensorController.deleteSensor);

module.exports = router;
