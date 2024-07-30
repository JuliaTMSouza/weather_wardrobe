const OutfitModel = require('../models/Outfits');
const { validateOutfit } = require('../validators/Outfits');

class OutfitController {
  // Cria um novo outfit
  static async createOutfit(req, res) {
    // Valida os dados do corpo da requisição
    const { error } = validateOutfit(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      // Cria o novo outfit e retorna a resposta
      const [newOutfit] = await OutfitModel.create(req.body);
      res.status(201).json(newOutfit);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém todos os outfits
  static async getAllOutfits(req, res) {
    try {
      // Obtém todos os outfits e retorna a resposta
      const outfits = await OutfitModel.getAll();
      res.status(200).json(outfits);
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Obtém um outfit por ID
  static async getOutfitById(req, res) {
    const { id } = req.params;
    try {
      // Obtém o outfit pelo ID e retorna a resposta
      const outfit = await OutfitModel.getById(id);
      if (outfit) {
        res.status(200).json(outfit);
      } else {
        res.status(404).json({ error: 'Outfit not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Atualiza um outfit por ID
  static async updateOutfit(req, res) {
    const { id } = req.params;
    // Valida os dados do corpo da requisição
    const { error } = validateOutfit(req.body);
    if (error) return res.status(400).json({ error: error.details[0].message });

    try {
      // Atualiza o outfit e retorna a resposta
      const updatedOutfit = await OutfitModel.update(id, req.body);
      if (updatedOutfit) {
        res.status(200).json(updatedOutfit);
      } else {
        res.status(404).json({ error: 'Outfit not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }

  // Deleta um outfit por ID
  static async deleteOutfit(req, res) {
    const { id } = req.params;
    try {
      // Deleta o outfit e retorna a resposta
      const deletedOutfit = await OutfitModel.delete(id);
      if (deletedOutfit) {
        res.status(204).send();
      } else {
        res.status(404).json({ error: 'Outfit not found' });
      }
    } catch (err) {
      res.status(500).json({ error: 'Internal Server Error' });
    }
  }
}

module.exports = OutfitController;
