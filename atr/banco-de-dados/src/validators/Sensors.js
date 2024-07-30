// validators/sensorValidator.js
const Joi = require('joi');

const sensorSchema = Joi.object({
  sensor_id: Joi.string().guid({ version: 'uuidv4' }).required(), // UUID para o ID do sensor
  machine_id: Joi.string().guid({ version: 'uuidv4' }).required(), // UUID para o ID da máquina, obrigatório
  sensor_type: Joi.string().valid('Temperature', 'Humidity', 'Pressure', 'Light', 'Other').required(), // Tipo do sensor
  data_type: Joi.string().valid('int', 'float', 'string').required(), // Tipo de dado coletado
  data_interval: Joi.number().integer().min(1).required() // Intervalo de tempo em segundos, obrigatório e deve ser um número inteiro positivo
});

function validateSensor(data) {
  return sensorSchema.validate(data);
}

module.exports = {
  validateSensor
};
