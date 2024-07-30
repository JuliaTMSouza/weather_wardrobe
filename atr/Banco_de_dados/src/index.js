const knex = require('knex');
const knexfile = require('../knexfile'); // Caminho para o arquivo de configuração do knex

// Configurar o knex
const db = knex(knexfile);
