// controllers/alertController.js
const AlertModel = require('../models/Alerts');
const { validateAlert } = require('../validators/Alerts');

class AlertController {
  // Cria um novo alerta
  static async createAlert(req, res) {
    const { error } = validateAlert(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const [newAlert] = await AlertModel.create(req.body);
      res.status(201).json(newAlert);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém todos os alertas
  static async getAllAlerts(req, res) {
    try {
      const alerts = await AlertModel.getAll();
      res.status(200).json(alerts);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém um alerta por ID
  static async getAlertById(req, res) {
    const { id } = req.params;
    try {
      const alert = await AlertModel.getById(id);
      if (alert) {
        res.status(200).json(alert);
      } else {
        res.status(404).json({ error: 'Alert not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Atualiza um alerta por ID
  static async updateAlert(req, res) {
    const { id } = req.params;
    const { error } = validateAlert(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const updatedAlert = await AlertModel.update(id, req.body);
      if (updatedAlert) {
        res.status(200).json(updatedAlert);
      } else {
        res.status(404).json({ error: 'Alert not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Deleta um alerta por ID
  static async deleteAlert(req, res) {
    const { id } = req.params;
    try {
      const deletedAlert = await AlertModel.delete(id);
      if (deletedAlert) {
        res.status(204).send();
      } else {
        res.status(404).json({ error: 'Alert not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }
}

module.exports = AlertController;
