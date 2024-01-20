const express = require('express');
const router = express.Router();

const auth  = require('../middleware/auth');

const biomesCtrl = require('../controllers/biomes');
router.get('/', auth, biomesCtrl.getAllBiomes);
router.post('/', auth, biomesCtrl.createBiome);
router.get('/:id', auth, biomesCtrl.getOneBiome);
router.put('/:id', auth, biomesCtrl.modifyBiome);
router.delete('/:id', auth, biomesCtrl.deleteBiome);
module.exports = router;
