// controllers/sensorController.js
const SensorModel = require('../models/Sensors');
const { validateSensor } = require('../validators/Sensors');

class SensorController {
  // Cria um novo sensor
  static async createSensor(req, res) {
    const { error } = validateSensor(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const [newSensor] = await SensorModel.create(req.body);
      res.status(201).json(newSensor);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém todos os sensores
  static async getAllSensors(req, res) {
    try {
      const sensors = await SensorModel.getAll();
      res.status(200).json(sensors);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém um sensor por ID
  static async getSensorById(req, res) {
    const { id } = req.params;
    try {
      const sensor = await SensorModel.getById(id);
      if (sensor) {
        res.status(200).json(sensor);
      } else {
        res.status(404).json({ error: 'Sensor not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Atualiza um sensor por ID
  static async updateSensor(req, res) {
    const { id } = req.params;
    const { error } = validateSensor(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const updatedSensor = await SensorModel.update(id, req.body);
      if (updatedSensor) {
        res.status(200).json(updatedSensor);
      } else {
        res.status(404).json({ error: 'Sensor not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Deleta um sensor por ID
  static async deleteSensor(req, res) {
    const { id } = req.params;
    try {
      const deletedSensor = await SensorModel.delete(id);
      if (deletedSensor) {
        res.status(204).send();
      } else {
        res.status(404).json({ error: 'Sensor not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }
}

module.exports = SensorController;
