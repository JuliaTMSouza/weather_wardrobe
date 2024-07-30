const Joi = require('joi');

const outfitSchema = Joi.object({
  name: Joi.string().required(),
  description: Joi.string().optional(),
  min_temperature: Joi.number().required(),
  max_temperature: Joi.number().required(),
  min_humidity: Joi.number().required(),
  max_humidity: Joi.number().required(),
  part_of_day: Joi.string().valid('Manhã', 'Tarde', 'Noite').optional()
});

function validateOutfit(data) {
  return outfitSchema.validate(data);
}

module.exports = {
  validateOutfit
};
