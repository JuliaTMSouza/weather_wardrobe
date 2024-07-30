const knex = require('knex')(require('../../../knexfile'));

class AlertModel {
  static async getAll() {
    return knex('Alerts').select('*');
  }

  static async getById(id) {
    return knex('Alerts').where('id', id).first();
  }

  static async create(alert) {
    return knex('Alerts').insert(alert).returning('*');
  }

  static async update(id, updates) {
    return knex('Alerts').where('id', id).update(updates).returning('*');
  }

  static async delete(id) {
    return knex('Alerts').where('id', id).del();
  }
}

module.exports = AlertModel;