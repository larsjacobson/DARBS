const mongoose = require('mongoose');

const agentSchema = mongoose.Schema({
  title: { type: String, required: true },
  userId: { type: String, required: true },
  locId: { type: String, required: false },
  userRating: { type: Number, required: false},
  description: { type: String, required: true}
});


module.exports = mongoose.model('Agent', agentSchema);
