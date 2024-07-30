const knex = require('knex')(require('../../../knexfile'));

class MachineModel {
  static async getAll() {
    return knex('Machines').select('*');
  }

  static async getById(id) {
    return knex('Machines').where('id', id).first();
  }

  static async create(machine) {
    return knex('Machines').insert(machine).returning('*');
  }

  static async update(id, updates) {
    return knex('Machines').where('id', id).update(updates).returning('*');
  }

  static async delete(id) {
    return knex('Machines').where('id', id).del();
  }
}

module.exports = MachineModel;