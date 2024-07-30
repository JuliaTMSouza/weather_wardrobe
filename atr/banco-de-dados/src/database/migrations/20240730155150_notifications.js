// migrations/xxxx_create_alerts_table.js
exports.up = function(knex) {
    return knex.schema.createTable('Notifications', function(table) {
      table.increments('id').primary(); // ID autoincrementado como chave primária
      table.string('name').notNullable(); // Nome do alerta (não pode ser nulo)
      table.text('description'); // Descrição do alerta
      table.string('alert_type'); // Tipo de alerta
      table.string('condition'); // Condição para o alerta
      table.float('temperature_min'); // Temperatura mínima do intervalo
      table.float('temperature_max'); // Temperatura máxima do intervalo
      table.float('humidity_min'); // Umidade mínima do intervalo
      table.float('humidity_max'); // Umidade máxima do intervalo
      table.float('uv_index'); // Índice UV
    });
  };
  
  exports.down = function(knex) {
    return knex.schema.dropTableIfExists('Notifications');
  };
  