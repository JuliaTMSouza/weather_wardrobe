const knex = require('knex')(require('../../../knexfile'));

class SensorReadingModel {
  static async getAll() {
    return knex('SensorReadings').select('*');
  }

  static async getById(id) {
    return knex('SensorReadings').where('id', id).first();
  }

  static async create(reading) {
    return knex('SensorReadings').insert(reading).returning('*');
  }

  static async update(id, updates) {
    return knex('SensorReadings').where('id', id).update(updates).returning('*');
  }

  static async delete(id) {
    return knex('SensorReadings').where('id', id).del();
  }
}

module.exports = SensorReadingModel;
