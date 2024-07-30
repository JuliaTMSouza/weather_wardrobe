const knex = require('knex')(require('../../knexfile'));

class SensorModel {
  static async getAll() {
    return knex('Sensors').select('*');
  }

  static async getById(id) {
    return knex('Sensors').where('id', id).first();
  }

  static async create(sensor) {
    return knex('Sensors').insert(sensor).returning('*');
  }

  static async update(id, updates) {
    return knex('Sensors').where('id', id).update(updates).returning('*');
  }

  static async delete(id) {
    return knex('Sensors').where('id', id).del();
  }
}

module.exports = SensorModel;
