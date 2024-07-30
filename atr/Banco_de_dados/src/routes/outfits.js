const express = require('express');
const OutfitController = require('../../controllers/Outfits');

const router = express.Router();

// Cria um novo outfit
router.post('/outfits', OutfitController.createOutfit);

// Obtém todos os outfits
router.get('/outfits', OutfitController.getAllOutfits);

// Obtém um outfit por ID
router.get('/outfits/:id', OutfitController.getOutfitById);

// Atualiza um outfit por ID
router.put('/outfits/:id', OutfitController.updateOutfit);

// Deleta um outfit por ID
router.delete('/outfits/:id', OutfitController.deleteOutfit);

module.exports = router;
