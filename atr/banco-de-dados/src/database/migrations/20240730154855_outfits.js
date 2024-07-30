// migrations/xxxx_create_conditions_table.js
exports.up = function(knex) {
    return knex.schema.createTable('Outfits', function(table) {
      table.increments('id').primary(); // ID autoincrementado como chave primária
      table.string('name').notNullable(); // Nome (não pode ser nulo)
      table.text('description'); // Descrição
      table.float('min_temperature'); // Temperatura mínima
      table.float('max_temperature'); // Temperatura máxima
      table.float('min_humidity'); // Umidade mínima
      table.float('max_humidity'); // Umidade máxima
      table.enu('part_of_day', ['Manhã', 'Tarde', 'Noite']).notNullable(); // Parte do dia (enum com opções)
    });
  };
  
  exports.down = function(knex) {
    return knex.schema.dropTableIfExists('Outfits');
  };
  