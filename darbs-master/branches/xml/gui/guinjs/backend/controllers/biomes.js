const Biome = require('../models/biome');

exports.createBiome = (req, res, next) => {
  const biome = new Biome({
    title: req.body.title,
    description: req.body.description,
    imageUrl: req.body.imageUrl,
    userId: req.body.userId,
    ratingNumber: req.body.ratingNumber

  });
  biome.save().then(
    () => {
      res.status(201).json({
        message: 'Post saved successfully!'
      });
    }
  ).catch(
    (error) => {
      res.status(400).json({
        error: error
      });
    }
  );
};

exports.getOneBiome = (req, res, next) => {
  Biome.findOne({
    _id: req.params.id
  }).then(
    (biome) => {
      res.status(200).json(biome);
    }
  ).catch(
    (error) => {
      res.status(404).json({
        error: error
      });
    }
  );
};

exports.modifyBiome = (req, res, next) => {
  const biome = new Biome({
    _id: req.params.id,
    title: req.body.title,
    description: req.body.description,
    imageUrl: req.body.imageUrl,
    userId: req.body.userId,
    ratingNumber: req.body.ratingNumber
  });
  Biome.updateOne({_id: req.params.id}, biome).then(
    () => {
      res.status(201).json({
        message: 'Biome updated successfully!'
      });
    }
  ).catch(
    (error) => {
      res.status(400).json({
        error: error
      });
    }
  );
};

exports.deleteBiome = (req, res, next) => {
  Biome.deleteOne({_id: req.params.id}).then(
    () => {
      res.status(200).json({
        message: 'Deleted!'
      });
    }
  ).catch(
    (error) => {
      res.status(400).json({
        error: error
      });
    }
  );
};

exports.getAllBiomes = (req, res, next) => {
  Biome.find().then(
    (biomes) => {
      res.status(200).json(biomes);
    }
  ).catch(
    (error) => {
      res.status(400).json({
        error: error
      });
    }
  );
};
