// validators/alertValidator.js
const Joi = require('joi');

const alertSchema = Joi.object({
  alert_id: Joi.string().guid({ version: 'uuidv4' }).required(), // UUID para o ID do alerta
  machine_id: Joi.string().guid({ version: 'uuidv4' }).optional(), // UUID para o ID da máquina
  sensor_id: Joi.string().guid({ version: 'uuidv4' }).optional(), // UUID para o ID do sensor
  alert_type: Joi.string().required(), // Tipo de alerta
  timestamp: Joi.date().iso().required(), // Data e hora do alerta
  description: Joi.string().optional() // Descrição opcional
});

function validateAlert(data) {
  return alertSchema.validate(data);
}

module.exports = {
  validateAlert
};
