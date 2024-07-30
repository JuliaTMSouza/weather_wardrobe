// controllers/sensorReadingController.js
const SensorReadingModel = require('../models/SensorReadings');
const { validateSensorReading } = require('../validators/SensorReadings');

class SensorReadingController {
  // Cria uma nova leitura de sensor
  static async createSensorReading(req, res) {
    const { error } = validateSensorReading(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const [newReading] = await SensorReadingModel.create(req.body);
      res.status(201).json(newReading);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém todas as leituras de sensores
  static async getAllSensorReadings(req, res) {
    try {
      const readings = await SensorReadingModel.getAll();
      res.status(200).json(readings);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém uma leitura de sensor por ID
  static async getSensorReadingById(req, res) {
    const { id } = req.params;
    try {
      const reading = await SensorReadingModel.getById(id);
      if (reading) {
        res.status(200).json(reading);
      } else {
        res.status(404).json({ error: 'Sensor Reading not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Atualiza uma leitura de sensor por ID
  static async updateSensorReading(req, res) {
    const { id } = req.params;
    const { error } = validateSensorReading(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const updatedReading = await SensorReadingModel.update(id, req.body);
      if (updatedReading) {
        res.status(200).json(updatedReading);
      } else {
        res.status(404).json({ error: 'Sensor Reading not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Deleta uma leitura de sensor por ID
  static async deleteSensorReading(req, res) {
    const { id } = req.params;
    try {
      const deletedReading = await SensorReadingModel.delete(id);
      if (deletedReading) {
        res.status(204).send();
      } else {
        res.status(404).json({ error: 'Sensor Reading not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }
}

module.exports = SensorReadingController;
