// validators/notificationValidator.js
const Joi = require('joi');

const notificationSchema = Joi.object({
  name: Joi.string().min(1).max(255).required(), // Nome do alerta, deve ter entre 1 e 255 caracteres
  description: Joi.string().optional(), // Descrição do alerta, opcional
  alert_type: Joi.string().valid('Temperature', 'Humidity', 'UV').required(), // Tipo de alerta
  condition: Joi.string().min(1).required(), // Condição para o alerta, obrigatória
  temperature_min: Joi.number().required(), // Temperatura mínima, obrigatória
  temperature_max: Joi.number().required(), // Temperatura máxima, obrigatória
  humidity_min: Joi.number().required(), // Umidade mínima, obrigatória
  humidity_max: Joi.number().required(), // Umidade máxima, obrigatória
  uv_index: Joi.number().optional() // Índice UV, opcional
});

function validateNotification(data) {
  return notificationSchema.validate(data);
}

module.exports = {
  validateNotification
};
