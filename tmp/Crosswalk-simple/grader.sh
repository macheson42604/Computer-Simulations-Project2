#!/usr/bin/env bash

ASSIGNNAME="crosswalksim"
EXPERS="2000"
METRICS="dadat s2adat dpdat"
GOLD_SEED=46347
GOLD_RUNS=25000
EXAMPLE_SEEDS="11473 483384 6363"
EXAMPLE_N=3
EXAMPLE_RUNS=300
GRADE_SEED=993748
GRADE_RUNS=400
rubric_line_item()
{
	local i=`case $1:$2 in
		*:dadat ) echo 0 ;; #nojacks_trash:dadat ) echo 1002;;
		*:s2adat ) echo 1 ;; #nojacks_trash:s2adat ) echo 1004;;
		*:dpdat ) echo 2 ;; #nojacks_trash:dpdat ) echo 1006;;
	esac`
	expr 7 + $i
}

missingdata=0
experiment()
{
	local theSIM="${1}"
	local arrivals="${2}"
	local RUNS="${3}"
	local SEED="${4}"		
	local missingdata=0
	local residfmt="__residual-%s-%s-%d.%s"
	local residautorand
	local residpedrand
	local residbuttonrand
	local residoutput

    echo -n >&2 $arrivals $RUNS " "
    for ((i=0, j=0; i<${RUNS}; i++, j+=3)); do 
        test $(( $i % 100 )) -eq 0 && echo -n >&2 $i " "
        residautorand=`printf ${5:-${residfmt}} $arrivals auto-random $i dat 2>/dev/null`
        residpedrand=`printf ${5:-${residfmt}} $arrivals ped-random $i dat 2>/dev/null`
        residbuttonrand=`printf ${5:-${residfmt}} $arrivals button-random $i dat 2>/dev/null`
        residoutput=`printf ${5:-${residfmt}} $arrivals output $i log 2>/dev/null`
        test_run -Q "${theSIM}" ${arrivals} \
                <(Random $(( SEED + i + j )) |tee "${residautorand}" )  \
                <(Random $(( SEED + i + j + 1 )) |tee "${residpedrand}" )  \
                <(Random $(( SEED + i + j + 2)) |tee "${residbuttonrand}" )  \
                | tee "${residoutput}" \
                | "${SIMGRADING}/output-pipe" | tr '\n' ' '  
        echo
    done | awk \
            -v dat1=__dadat-${arrivals}-$SEED-$RUNS.dat \
            -v dat2=__s2adat-${arrivals}-$SEED-$RUNS.dat \
            -v dat3=__dpdat-${arrivals}-$SEED-$RUNS.dat \
            '{ print $1 >dat1; print $2 >dat2; print $3 >dat3; }'
	echo ${RUNS} >&2

	for m in ${METRICS} ; do 
	    test `ncdat "__${m}-${arrivals}-${SEED}-${RUNS}.dat"` -ne ${RUNS} && missingdata=1   
	done

	for m in ${METRICS} ; do 
		for x in _${m}-${arrivals}-${SEED}-${RUNS}.dat ; do
			lineswithdata "_${x}" | sort -g |${SIMGRADING}/ecdf >${x}
		done
	done


	return ${missingdata}
}


###
# SIMS and usage should be defined before sourcing sim-lib.sh
###
SIMS=crosswalksim
usage()
{
	cat <<EoT
ASSIGNMENT SPECIFIC OPTIONS 

  $ /assignment/specific/path/grader.sh . [RUNS [SEED [ARRIVALS]]]

Where

  . is the SIM location (required first parameter)

  RUNS is the number of RUNS to use for SIM execution and plot generation

  SEED is a positive integer for a SEED to use for random file inputs

  ARRIVALS is a positive integer for the number of arrivals (provided
           directly to your SIM

To retain residual data files to (maybe) assist in debugging, export

  $ export GRADER_SAVE_RESIDUALS=${SIMS}

EoT
}

# if SIMGRADING is unknown
test -z "${SIMGRADING}" -a -r ~khellman/SIMGRADING/sim-lib.sh && ~khellman/SIMGRADING/setup.sh ~khellman/SIMGRADING
if ! test -r "${SIMGRADING}/sim-lib.sh" ; then
	cat >&2 <<EoT
ERROR:  SIMGRADING is not in your environment or SIMGRADING/sim-lib.sh cannot
be found.  Have you followed the grader tarball setup.sh instructions on the
assignment's Wiki page?
EoT
	exit 1
fi

test -n "${SIMGRADING}" && source "${SIMGRADING}/sim-lib.sh"
source "${GRADERDIR}/grader-exec.sh"

set -e

RUNS=${1:-${GRADE_RUNS}}
SEED=${2:-${GRADE_SEED}}
EXPERS=${3:-${EXPERS}}

tallydir=`create_tally_dir 1 TYPE=auto MAX=10 "Handles command line arguments properly"`
cat <<EoT |grader_msg 
Running SIM with non-existent trace file.  These runs should exit with non-zero
status and without OUTPUT lines...
EoT
set +e   # disregard exit status
for S in ${SIMS} ; do 
	tally_nonexist_tracefile "${tallydir}/dne-auto" "${simloc}/SIM" 2000 MISSINGRANDOM <(Random 1) <(Random 2)
	tally_nonexist_tracefile "${tallydir}/dne-ped" "${simloc}/SIM" 2000 <(Random 1) MISSINGRANDOM <(Random 2)
	tally_nonexist_tracefile "${tallydir}/dne-button" "${simloc}/SIM" 2000 <(Random 1) <(Random 2) MISSINGRANDOM 
done


grader_msg <<EoT
Running SIM with truncated trace file.  These runs should exit with non-zero
status and without OUTPUT lines...
EoT
set +e   # disregard exit status
for S in ${SIMS} ; do 
	tally_truncated_tracefile "${tallydir}/trunc-auto" "${simloc}/SIM" 2000 TRUNCRANDOM <(Random 1) <(Random 2) 
	tally_truncated_tracefile "${tallydir}/trunc-ped" "${simloc}/SIM" 2000 <(Random 1) TRUNCRANDOM <(Random 2) 
	tally_truncated_tracefile "${tallydir}/trunc-button" "${simloc}/SIM" 2000 <(Random 1) <(Random 2) TRUNCRANDOM
done


# create rubric line items for 2-5 DOC rubric line items
(
	create_tally_dir 2 TYPE=man MAX=10 "Pedestrian arrivals meets requirements"
	create_tally_dir 3 TYPE=man MAX=10 "Automobile arrivals meets requirements"
	create_tally_dir 4 TYPE=man MAX=10 "Welford implementation and two 'adding an xi' locations"
	create_tally_dir 5 TYPE=man MAX=5 "Exponential(u) implementation"
	create_tally_dir 6 TYPE=man MAX=5 "Prioritized event list data structure used"
) >/dev/null 


# create tally descriptions for EXPERS x METRIC rubrics
(
	create_tally_dir 7 TYPE=man MAX=10 "D_A results (CDF) consistent with grader.tar.bz2"
	create_tally_dir 8 TYPE=man MAX=10 "s2A results (CDF) consistent with grader.tar.bz2"
	create_tally_dir 9 TYPE=man MAX=10 "D_P results (CDF) consistent with grader.tar.bz2"
) >/dev/null

grader_msg <<EoT
Running many SIM experiments with varying seeds.  This may take 
some time ...
EoT
set -e
missingdata=0
for S in ${EXPERS} ; do
	experiment "${simloc}/SIM" ${S} ${RUNS} ${SEED}
	missingdata=$?

    grader_msg <<EoT
... generating comparison plots for ${S} ...
EoT
    for x in ${METRICS} ; do 
		gplotpdf ${x} ${S} ${RUNS} ${SEED} `rubric_line_item $S $x`
    done

	grader_cleanup_experiment ${missingdata} "${METRICS}" "${S}" "${RUNS}" "${SEED}"

    grader_save_residuals ${S}

done

grader_signoff ${RUNS} ${SEED}
