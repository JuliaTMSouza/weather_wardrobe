const express = require('express');

const machines = require('./machines');
const sensors = require('./sensors');
const sensorReadings = require('./sensorReadings');
const notifications = require('./notifications');
const outfits = require('./outfits');
const alerts = require('./alerts');

const routes = express.Router();

routes.use('/machines', machines);
routes.use('/sensores', sensors);
routes.use('/sensorReadings', sensorReadings);
routes.use('/notifications', notifications);
routes.use('/outfits', outfits);
routes.use('/alerts', alerts);

module.exports = routes;
