const knex = require('knex')(require('../../knexfile'));

class OutfitModel {
  static async getAll() {
    return knex('Outfits').select('*');
  }

  static async getById(id) {
    return knex('Outfits').where('id', id).first();
  }

  static async create(outfit) {
    return knex('Outfits').insert(outfit).returning('*');
  }

  static async update(id, updates) {
    return knex('Outfits').where('id', id).update(updates).returning('*');
  }

  static async delete(id) {
    return knex('Outfits').where('id', id).del();
  }
}

module.exports = OutfitModel;
