exports.up = function(knex) {
    return knex.schema.createTable('Machines', function(table) {
      table.text('machine_id').primary();
      table.text('name');
      table.text('location');
    });
  };
  
  exports.down = function(knex) {
    return knex.schema.dropTable('Machines');
  };
