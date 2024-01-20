const express = require('express');
const router = express.Router();

const auth  = require('../middleware/auth');

const agentsCtrl = require('../controllers/agents');
router.get('/', auth, agentsCtrl.getAllAgents);
router.post('/', auth, agentsCtrl.createAgent);
router.get('/:id', auth, agentsCtrl.getOneAgent);
router.put('/:id', auth, agentsCtrl.modifyAgent);
router.delete('/:id', auth, agentsCtrl.deleteAgent);
module.exports = router;
