const express = require('express');
const AlertController = require('../controllers/Alerts');

const router = express.Router();

// Cria um novo alerta
router.post('/alerts', AlertController.createAlert);

// Obtém todos os alertas
router.get('/alerts', AlertController.getAllAlerts);

// Obtém um alerta por ID
router.get('/alerts/:id', AlertController.getAlertById);

// Atualiza um alerta por ID
router.put('/alerts/:id', AlertController.updateAlert);

// Deleta um alerta por ID
router.delete('/alerts/:id', AlertController.deleteAlert);

module.exports = router;
