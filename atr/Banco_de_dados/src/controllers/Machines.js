// controllers/machineController.js
const MachineModel = require('../routes/models/Machines');
const { validateMachine } = require('../../validators/Machines');

class MachineController {
  // Cria uma nova máquina
  static async createMachine(req, res) {
    const { error } = validateMachine(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const [newMachine] = await MachineModel.create(req.body);
      res.status(201).json(newMachine);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém todas as máquinas
  static async getAllMachines(req, res) {
    try {
      const machines = await MachineModel.getAll();
      res.status(200).json(machines);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém uma máquina por ID
  static async getMachineById(req, res) {
    const { id } = req.params;
    try {
      const machine = await MachineModel.getById(id);
      if (machine) {
        res.status(200).json(machine);
      } else {
        res.status(404).json({ error: 'Machine not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Atualiza uma máquina por ID
  static async updateMachine(req, res) {
    const { id } = req.params;
    const { error } = validateMachine(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      const updatedMachine = await MachineModel.update(id, req.body);
      if (updatedMachine) {
        res.status(200).json(updatedMachine);
      } else {
        res.status(404).json({ error: 'Machine not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Deleta uma máquina por ID
  static async deleteMachine(req, res) {
    const { id } = req.params;
    try {
      const deletedMachine = await MachineModel.delete(id);
      if (deletedMachine) {
        res.status(204).send();
      } else {
        res.status(404).json({ error: 'Machine not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }
}

module.exports = MachineController;
