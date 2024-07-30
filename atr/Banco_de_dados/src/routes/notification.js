const express = require('express');
const NotificationController = require('../controllers/notificationController');

const router = express.Router();

// Cria uma nova notificação
router.post('/notifications', NotificationController.createNotification);

// Obtém todas as notificações
router.get('/notifications', NotificationController.getAllNotifications);

// Obtém uma notificação por ID
router.get('/notifications/:id', NotificationController.getNotificationById);

// Atualiza uma notificação por ID
router.put('/notifications/:id', NotificationController.updateNotification);

// Deleta uma notificação por ID
router.delete('/notifications/:id', NotificationController.deleteNotification);

module.exports = router;
