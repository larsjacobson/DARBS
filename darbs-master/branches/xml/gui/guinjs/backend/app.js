

const express = require('express');

const app = express();

const bodyParser = require('body-parser');

const mongoose = require('mongoose');

const Biome = require('./models/biome');

const biomesRoutes = require('./routes/biomes');

const Agent = require('./models/agent');

const agentsRoutes = require('./routes/agents');

const userRoutes  = require('./routes/user');

mongoose.connect('mongodb+srv://root:root@team3d-pscfn.mongodb.net/test?retryWrites=true')
  .then(() => {
    console.log('Successfully connected to team3d MongoDB Atlas!');
  })
  .catch((error) => {
    console.log('Unable to connect to MongoDB Atlas!');
    console.error(error);
  });

app.use((req, res, next) => {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content, Accept, Content-Type, Authorization');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, PATCH, OPTIONS');
  next();
});

app.use(bodyParser.json());

app.use('/api/biomes', biomesRoutes);
app.use('/api/agents', agentsRoutes);

app.use('/api/auth',  userRoutes);




module.exports = app;
