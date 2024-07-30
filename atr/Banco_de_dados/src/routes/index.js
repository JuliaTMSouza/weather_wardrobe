const express = require('express');

const machines = require('./usuario');
const sensors = require('./treinador');
const sensorReadings = require('./modalidade');
const notifications = require('./atleta');
const outfits = require('./professor');
const alerts = require('./aluno');

const routes = express.Router();

routes.use('/machines', machines);
routes.use('/sensores', sensors);
routes.use('/sensorReadings', sensorReadings);
routes.use('/notifications', notifications);
routes.use('/outfits', outfits);
routes.use('/alerts', alerts);

module.exports = routes;
