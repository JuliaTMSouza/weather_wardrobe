exports.up = function(knex) {
    return knex.schema.createTable('Sensors', function(table) {
      table.text('sensor_id').primary();
      table.text('machine_id').references('machine_id').inTable('Machines').onDelete('CASCADE');
      table.text('sensor_type');
      table.text('data_type');
      table.integer('data_interval');
    });
  };
  
  exports.down = function(knex) {
    return knex.schema.dropTable('Sensors');
  };