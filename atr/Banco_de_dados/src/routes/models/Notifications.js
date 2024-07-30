const knex = require('knex')(require('../../../knexfile'));

class NotificationModel {
  static async getAll() {
    return knex('Notificaions').select('*');
  }

  static async getById(id) {
    return knex('Notificaions').where('id', id).first();
  }

  static async create(notification) {
    return knex('Notificaions').insert(notification).returning('*');
  }

  static async update(id, updates) {
    return knex('Notificaions').where('id', id).update(updates).returning('*');
  }

  static async delete(id) {
    return knex('Notificaions').where('id', id).del();
  }
}

module.exports = NotificationModel;