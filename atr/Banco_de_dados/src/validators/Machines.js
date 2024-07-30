// validators/machineValidator.js
const Joi = require('joi');

const machineSchema = Joi.object({
  machine_id: Joi.string().guid({ version: 'uuidv4' }).required(), // UUID para o ID da máquina
  name: Joi.string().min(1).max(255).required(), // Nome da máquina, deve ter entre 1 e 255 caracteres
  location: Joi.string().min(1).max(255).optional() // Localização da máquina, opcional
});

function validateMachine(data) {
  return machineSchema.validate(data);
}

module.exports = {
  validateMachine
};
