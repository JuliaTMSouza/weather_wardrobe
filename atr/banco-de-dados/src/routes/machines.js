const express = require('express');
const MachineController = require('../controllers/Machines');

const router = express.Router();

// Cria uma nova máquina
router.post('/machines', MachineController.createMachine);

// Obtém todas as máquinas
router.get('/machines', MachineController.getAllMachines);

// Obtém uma máquina por ID
router.get('/machines/:id', MachineController.getMachineById);

// Atualiza uma máquina por ID
router.put('/machines/:id', MachineController.updateMachine);

// Deleta uma máquina por ID
router.delete('/machines/:id', MachineController.deleteMachine);

module.exports = router;
