const fs = require('fs');
const path = require('path');
const knex = require('knex')({
  client: 'sqlite3',
  connection: {
    filename: path.resolve(__dirname, './database/mydatabase.db') // Caminho absoluto para o banco de dados SQLite
  },
  useNullAsDefault: true // SQLite requer essa opção
});

// Caminho absoluto para o arquivo SQL
const inserirDados = async () => {
  try {
    // Caminhos absolutos para os arquivos SQL
    const outfitsSqlFilePath = path.resolve(__dirname, './database/inserir-outfits.sql');
    const alertasSqlFilePath = path.resolve(__dirname, './database/inserir-alertas.sql');

    // Leitura dos arquivos SQL
    const outfitsSql = fs.readFileSync(outfitsSqlFilePath, 'utf8');
    const alertasSql = fs.readFileSync(alertasSqlFilePath, 'utf8');

    // Execução dos comandos SQL
    await knex.raw(outfitsSql);
    console.log('Dados de outfits inseridos com sucesso!');

    await knex.raw(alertasSql);
    console.log('Dados de alertas inseridos com sucesso!');
  } catch (err) {
    console.error('Erro ao inserir dados:', err);
  } finally {
    knex.destroy(); // Fecha a conexão com o banco de dados
  }
};

// Chama a função para inserir os dados
inserirDados();

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
