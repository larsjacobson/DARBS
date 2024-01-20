window.addEvent("domready", function() {

    var Synth = new Class({
        Extends: AudioletGroup,
        initialize: function(audiolet, frequency) {
            AudioletGroup.prototype.initialize.apply(this, [audiolet, 0, 1]);

            // Sine base oscillator
            this.sine = new Sine(audiolet, frequency);

            // Gain envelope
            this.gainEnv = new PercussiveEnvelope(audiolet, 1, 0.05, 0.5,
                function() {
                    this.audiolet.scheduler.addRelative(0,
                                                        this.remove.bind(this));
                }.bind(this)
            );

            this.gainEnvMulAdd = new MulAdd(audiolet, 0.5);
            this.gain = new Gain(audiolet);

            // Connect oscillator
            this.sine.connect(this.gain);

            this.gainEnv.connect(this.gainEnvMulAdd);
            this.gainEnvMulAdd.connect(this.gain, 0, 1);
            this.gain.connect(this.outputs[0]);
        }
    });

    var AudioletApp = new Class({
        initialize: function() {
            this.audiolet = new Audiolet();

            this.cfForm = $('cantusFirmus');
            this.cfButton = this.cfForm.getChildren('button');
            this.cf = this.cfForm.getChildren('input[type=text]');
            this.cfButton.addEvent('click', this.cfCallback.bind(this));

            this.cpForm = $('counterpoint');
            this.cpButton = this.cpForm.getChildren('button');
            this.cp = this.cpForm.getChildren('input[type=text]');
            this.cpButton.addEvent('click', this.cpCallback.bind(this));

            this.bothForm = $('both');
            this.bothButton = this.bothForm.getChildren('button');
            this.bothButton.addEvent('click', this.bothCallback.bind(this));
        },

        cfCallback: function() {
            var cantusFirmus = this.cf.get("value")[0];
            cantusFirmus = cantusFirmus.split(" ");
            var notePattern = new PSequence(cantusFirmus);
            this.audiolet.scheduler.play([notePattern], 1,
                                         this.playNote.bind(this));
        },

        cpCallback: function() {
            var counterpoint = this.cp.get("value")[0];
            counterpoint = counterpoint.split(" ");
            var notePattern = new PSequence(counterpoint);
            this.audiolet.scheduler.play([notePattern], 1,
                                         this.playNote.bind(this));
        },

        bothCallback: function() {
            this.cfCallback();
            this.cpCallback();
        },

        playNote: function(note) {
            var frequency = 440 * Math.pow(2, (note - 57) / 12);
            var synth = new Synth(this.audiolet, frequency);
            synth.connect(this.audiolet.output);
        }
    });

    this.audioletApp = new AudioletApp();
});


