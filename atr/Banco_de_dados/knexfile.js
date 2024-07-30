// Update with your config settings.

/**
 * @type { Object.<string, import("knex").Knex.Config> }
 */
module.exports = {
  client: 'sqlite3',
  connection: {
    filename: './mydatabase.db' // Caminho para o arquivo do banco de dados SQLite
  },
  useNullAsDefault: true // Necessário para o SQLite
};