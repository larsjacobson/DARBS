const Agent = require('../models/agent');

exports.createAgent = (req, res, next) => {
  const agent = new Agent({
    title: req.body.title,
    description: req.body.description,
    userId: req.body.userId,
    locId: req.body.locId
  });

  agent.save().then(
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

exports.getOneAgent = (req, res, next) => {
  Agent.findOne({
    _id: req.params.id
  }).then(
    (agent) => {
      res.status(200).json(agent);
    }
  ).catch(
    (error) => {
      res.status(404).json({
        error: error
      });
    }
  );
};

exports.modifyAgent = (req, res, next) => {
  const agent = new Agent({
    _id: req.params.id,
    title: req.body.title,
    description: req.body.description,
    userId: req.body.userId,
    locId: req.body.locId
  });
  Agent.updateOne({_id: req.params.id}, agent).then(
    () => {
      res.status(201).json({
        message: 'Agent updated successfully!'
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

exports.deleteAgent = (req, res, next) => {
  Agent.deleteOne({_id: req.params.id}).then(
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
//
// exports.getAllAgents = (req, res, next) => {
//   Agent.find().then(
//     (agents) => {
//       res.status(200).json(agents);
//     }
//   ).catch(
//     (error) => {
//       res.status(400).json({
//         error: error
//       });
//     }
//   );
// };
exports.getAllAgents = (req, res, next) => {
  Agent.find().then(
    (agents) => {
      res.status(200).json(agents);
    }
  ).catch(
    (error) => {
      res.status(400).json({
        error: error
      });
    }
  );
};
