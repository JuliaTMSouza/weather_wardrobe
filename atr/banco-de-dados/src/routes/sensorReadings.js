const express = require('express');
const SensorReadingController = require('../controllers/SensorReadings');

const router = express.Router();

// Cria uma nova leitura de sensor
router.post('/sensorReadings', SensorReadingController.createSensorReading);

// Obtém todas as leituras de sensores
router.get('/sensorReadings', SensorReadingController.getAllSensorReadings);

// Obtém uma leitura de sensor por ID
router.get('/sensorReadings/:id', SensorReadingController.getSensorReadingById);

// Atualiza uma leitura de sensor por ID
router.put('/sensorReadings/:id', SensorReadingController.updateSensorReading);

// Deleta uma leitura de sensor por ID
router.delete('/sensorReadings/:id', SensorReadingController.deleteSensorReading);

module.exports = router;
