const mongoose = require('mongoose');

const biomeSchema = mongoose.Schema({
  title: { type: String, required: true },
  description: { type: String, required: true },
  imageUrl: { type: String, required: true },
  userId: { type: String, required: true },
  ratingNumber: { type: Number, required: false }
});

module.exports = mongoose.model('Biome', biomeSchema);
