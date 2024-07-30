// const knex = require('knex');
// const knexfile = require('../knexfile');
const routes = require("./routes");
const cors = require("cors"); // Caminho para o arquivo de configuração do knex
const { errors } = require("celebrate");
const express = require('express');
const port = 3001; // Porta onde o backend estará escutando

const app = express();
app.use(express.static("public"));
const corsOptions = {
  exposedHeaders: "X-Total-Count"
};

app.use(cors(corsOptions));
app.use(express.json());
app.use(routes);
app.use(errors());

app.use("/", (req, res) => {
  return res.status(200).json({
    notification: "Ok, server MonitoraApp is running!"
  });
});

app.listen(port, () => {
  console.log(`Listening on port: ${port}`);
});
