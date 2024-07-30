exports.up = function(knex) {
    return knex.schema.createTable('SensorReadings', function(table) {
      table.text('reading_id').primary();
      table.text('sensor_id').references('sensor_id').inTable('Sensors').onDelete('CASCADE');
      table.datetime('timestamp');
      table.float('value');
    });
  };
  
  exports.down = function(knex) {
    return knex.schema.dropTable('SensorReadings');
  };
