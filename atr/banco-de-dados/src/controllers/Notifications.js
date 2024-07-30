// controllers/notificationController.js
const NotificationModel = require('../models/Notifications');
const { validateNotification } = require('../validators/Notifications');

class NotificationController {
  // Cria uma nova notificação
  static async createNotification(req, res) {
    const { error } = validateNotification(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const [newNotification] = await NotificationModel.create(req.body);
      res.status(201).json(newNotification);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém todas as notificações
  static async getAllNotifications(req, res) {
    try {
      const notifications = await NotificationModel.getAll();
      res.status(200).json(notifications);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém uma notificação por ID
  static async getNotificationById(req, res) {
    const { id } = req.params;
    try {
      const notification = await NotificationModel.getById(id);
      if (notification) {
        res.status(200).json(notification);
      } else {
        res.status(404).json({ error: 'Notification not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Atualiza uma notificação por ID
  static async updateNotification(req, res) {
    const { id } = req.params;
    const { error } = validateNotification(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const updatedNotification = await NotificationModel.update(id, req.body);
      if (updatedNotification) {
        res.status(200).json(updatedNotification);
      } else {
        res.status(404).json({ error: 'Notification not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Deleta uma notificação por ID
  static async deleteNotification(req, res) {
    const { id } = req.params;
    try {
      const deletedNotification = await NotificationModel.delete(id);
      if (deletedNotification) {
        res.status(204).send();
      } else {
        res.status(404).json({ error: 'Notification not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }
}

module.exports = NotificationController;
