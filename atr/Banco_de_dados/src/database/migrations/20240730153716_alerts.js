exports.up = function(knex) {
    return knex.schema.createTable('Alerts', function(table) {
      table.text('alert_id').primary();
      table.text('machine_id').references('machine_id').inTable('Machines').onDelete('CASCADE');
      table.text('sensor_id').references('sensor_id').inTable('Sensors').onDelete('SET NULL');
      table.text('alert_type');
      table.datetime('timestamp');
      table.text('description');
    });
  };
  
  exports.down = function(knex) {
    return knex.schema.dropTable('Alerts');
  };