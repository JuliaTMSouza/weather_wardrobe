// validators/sensorReadingValidator.js
const Joi = require('joi');

const sensorReadingSchema = Joi.object({
  reading_id: Joi.string().guid({ version: 'uuidv4' }).required(), // UUID para o ID da leitura
  sensor_id: Joi.string().guid({ version: 'uuidv4' }).required(), // UUID para o ID do sensor, obrigatório
  timestamp: Joi.date().iso().required(), // Data e hora da leitura em formato ISO 8601
  value: Joi.number().required() // Valor da leitura, obrigatório e deve ser um número
});

function validateSensorReading(data) {
  return sensorReadingSchema.validate(data);
}

module.exports = {
  validateSensorReading
};
